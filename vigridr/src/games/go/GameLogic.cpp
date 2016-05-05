#include "GameLogic.h"
#include <iostream>
#include <stdio.h>

namespace mjollnir { namespace vigridr {

/************************/
/************************/
/************************/
/************************/
/************************/
/************************/
/****   GAME_LOGIC   ****/
/************************/
/************************/
/************************/
/************************/
/************************/
/************************/

GameLogic::GameLogic(int32_t playerId1, int32_t playerId2) {
  player1_ = playerId1;
  player2_ = playerId2;
  winner_ = "-1";
  hasFinished_ = false;

  std::vector<std::vector<Marker> > table;
  for (int i = 0; i < boardSize_; i++) {
    std::vector<Marker> line;    
    for (int i = 0; i < boardSize_; i++) {
      line.push_back(Marker::UNMARKED);
    }
    table.push_back(line);
  }

  worldModel_.table = table;

  bMove_ = 0;
  wMove_ = 0;

  //initialize captured stones count to 0
  blackPrisoners_ = 0;
  whitePrisoners_ = 0;

  //set both players intial scores
  scoreBlack_ = 0;
  scoreWhite_ = 0;

  //set to -1 when there is no KO on the board
  koPosX_ = -1;
  koPosY_ = -1;

  blackStonesNumber_ = initialBlackStonesNumber_;
  whiteStonesNumber_ = initialWhiteStonesNumber_;
 
  //make space for the grid_
  grid_ = new Stone **[boardSize_];
  for( int x = 0; x<boardSize_; x++ ){
    grid_[x] = new Stone *[boardSize_];
    for( int y = 0; y < boardSize_; y++ ){
      grid_[x][y] = new Stone( EMPTY, x, y );
    }
  }
}

// TODO: Change logic of /Mjollnir/vigridr/src/server/GameManager.cpp to receive
// from GemeLogic::update two booleans, one that indicates invalid command and another
// that indicates end of match
bool GameLogic::update(Command command, int32_t playerId) {
  if(!hasFinished_ && isValidMove_(command.coordinate.x, command.coordinate.y)) {
    if (playerId == player1_) {
      if (!isPassMove_(command.coordinate.x, command.coordinate.y)) {
        //perform black's move
        bMove_ = step_(BLACK, command.coordinate);
        blackStonesNumber_--;
      }
      else {
        bMove_ = -1;
      }
    }
    else if (playerId == player2_) {
      if (!isPassMove_(command.coordinate.x, command.coordinate.y)) {
        //perform white's move
        wMove_ = step_(WHITE, command.coordinate);
        whiteStonesNumber_--;
      }
      else {
        wMove_ = -1;
      }
    }

    updateWorldModel_();

    if( matchFinished_() ) {
      hasFinished_ = true;
      determineWinner_();
    }

    return true;
  }
  return false;
}

GameDescription GameLogic::getGameDescription(int32_t playerId) const {
  GameDescription gameDescription;
  gameDescription.myType = (playerId == player1_) ? Marker::X : Marker::O;
  return gameDescription;
}

WorldModel GameLogic::getWorldModel() const {
  return worldModel_;
}

void GameLogic::setTableCoordinate(
    const Coordinate& coordinate, Marker marker) {
  setTableCoordinate_(coordinate, marker);
}

void GameLogic::setTableCoordinate_(
    const Coordinate& coordinate, Marker marker) {
  worldModel_.table[coordinate.x][coordinate.y] = marker;
}

bool GameLogic::isFinished() const {
  return hasFinished_;
}

void GameLogic::setHasFinished(bool value) {
  hasFinished_ = value;
}
  
std::string GameLogic::getWinner() const {
  return winner_;
}

void GameLogic::setWinner(std::string value) {
  winner_ = value;
}

// Can delete this method?
bool GameLogic::randomPlay_(int32_t playerId) {
  for(auto& line : worldModel_.table)
    for(auto& element : line)
      if(element == Marker::UNMARKED) {
        if(playerId == player1_) {
          element = Marker::X;
          return true;
        }
        else if(playerId == player2_) {
          element = Marker::O;
          return true;
        }
      }
  return false;
}

bool GameLogic::shouldPrintWorldModel(int32_t playerId){
  return true;
}

bool GameLogic::shouldIncrementCycle(int32_t playerId){
  return true;
}

size_t GameLogic::getNumberOfPlayers() const {
  return numberOfPlayers_;
}

TotalWorldModel GameLogic::getTotalWorldModel() const {
  return twm_;
}

GameResult GameLogic::createGameResult(std::string result, int32_t id) {
  if (result == std::to_string(id)) {
    return GameResult::WON;
  }
  if (result == "-1") {
    return GameResult::TIED;
  }
  return GameResult::LOST;
}

void GameLogic::updateWorldModel_(){
  Command command;
  for(int i =0;i < boardSize_; i++){
    for(int j = 0; j < boardSize_; j++){
      command.coordinate.x = i;
      command.coordinate.y = j;
      switch(grid_[i][j]->getPlayer()){
        case BLACK:
           setTableCoordinate_(command.coordinate, Marker::X);
           break;
        case WHITE:
           setTableCoordinate_(command.coordinate, Marker::O);
           break;
        default:
           setTableCoordinate_(command.coordinate, Marker::UNMARKED);
           break;
      }
    }
  }
}

bool GameLogic::isOnBoard_(int x, int y)
{
  // check that move is inside the board space
  if (x >= 0 && x < boardSize_ &&
      y >= 0 && y < boardSize_) {
    return true;
  }    
    return false;
}

bool GameLogic::isKo_(int32_t x, int32_t y)
{
  // check if move is equivalent to the previous game state
  if (x == koPosX_ && y == koPosY_)
  {
    return true;
  }
  return false;
}

bool GameLogic::isPlayer_(
    int32_t x, int32_t y, Marker marker) {
  return worldModel_.table[x][y] == marker;
}

bool GameLogic::isValidMove_(int32_t x, int32_t y)
{
  // passing turn
  if (isPassMove_(x, y))
    return true;
  if (isOnBoard_(x, y) &&
      isPlayer_(x, y, Marker::UNMARKED) &&
      !isKo_(x, y)) {
    return true;
  }
  return false;
}

bool GameLogic::isPassMove_(int32_t x, int32_t y) {
  return x == -1 && y == -1;
}

bool GameLogic::matchFinished_(){
  //check for 2 passes in a row
  if( whiteStonesNumber_ == 0 && blackStonesNumber_ == 0 )
    return true;
  if( bMove_ == -1 && wMove_ == -1 )
    return true;
  return false;
}

void GameLogic::determineWinner_(){
  //determine the winner
  Player winner = scoreArea_();

  switch( winner ){
    case( BLACK ):
      winner_ = std::to_string(player1_);
      break;
    case( WHITE ):
      winner_ = std::to_string(player2_);
      break;
    default:
      winner_ = "-1";
      break;
  }
}

/*
  core function, drives all action on the board each turn

  param player: the player whose turn needs to be executed

  param x,y: coordinates of the stone to be placed
             (note, should be validated with isValidMove() before
       step is called.
   
  return int: a signal to show the turn has completed succesfully
*/
int GameLogic::step_( Player player, const Coordinate& coordinate ){
  Player currPlayer = player;
  Player Enemy;
  Stone *currStone;
  bool KOflag1 = false;
  Group playerGroup;
  int x = coordinate.x;
  int y = coordinate.y;
 
  std::string playerName;

  //establish the opposing player for reference
  if( currPlayer == BLACK ){
    Enemy = WHITE;
  } 
  else{
    Enemy = BLACK;
  }
 
  //get the stone at x, y
  currStone = grid_[x][y];
  
  //set the piece to its new player
  currStone->setPlayer( currPlayer ); 

  //test and possibly kill enemies
  KOflag1 = effectEnemies_( Enemy, x, y ); //set the Ko flag to the output

  //initialize the attributes of the group for the current player
  playerGroup.size = 0;
  playerGroup.liberties = 0;
  playerGroup.player = currPlayer;
  playerGroup.owner = (Player)EMPTY;
  //call to determine the attributes
  playerGroup = getGroup_( x, y, playerGroup );

  //test if player killed themselves
  if( playerGroup.liberties == 0 ){
    clearGroup_( currPlayer, x, y );
  }

  //Determine if there is not a KO
  if( playerGroup.liberties != 1 || playerGroup.size != 1 || !KOflag1 ){
    //The KO positions are set when an enemy is killed,
    //if a KO is not possible then the positions need to be reset 
    //to an unreachable position -1,-1 before the step finishes
    koPosX_ = -1;
    koPosY_ = -1;
  }
  
  //clear all our Counted marks
  clearMarks_();

  //return to signal that the player made a move
  return 1;  
}

/*
  Probes adjecent stones and determines if any neighbor groups
  no longer have liberties

  param enemy: the color of the opponents stones
  param x, y: coordinates of the newly placed stone

  return Bool, KOflag, returns true if a state of KO may be possible
  this flag, combined with the flag in the main step function, determines
  if there is in fact a KO state.
*/
bool GameLogic::effectEnemies_( Player enemy, int x, int y ){  
  //flags to see if a state of KO may be possible
  int killSize = 0; //size of last gorup killed, used to test for KO, and scoring
  int numKilled = 0; //groups killed, used to test for KO

  //create a group to be reused to test all enemies
  Group enemyGroup;
  enemyGroup.player = enemy;
  enemyGroup.owner = (Player)EMPTY;
  enemyGroup.size = 0;
  enemyGroup.liberties = 0;

  //array's to represent orthogonal neighbors
  int xnew[4] = { x-1, x, x+1, x };
  int ynew[4] = { y, y-1, y, y+1 };

  //iterate through each neighbor
  for( int i = 0; i < 4; i++ ){
    //make sure we don't go outside the board
    if( isOnBoard_( xnew[i], ynew[i] )){
      //see if the stone is an enemy and has NOT been counted yet
      if( grid_[xnew[i]][ynew[i]]->isPlayer( enemy ) && 
          !grid_[xnew[i]][ynew[i]]->isCounted() ){
        enemyGroup.size = 0;
        enemyGroup.liberties = 0;
        enemyGroup = getGroup_( xnew[i], ynew[i], enemyGroup );
        if( enemyGroup.liberties == 0 ){
          //set the killSize when a group is killed
          killSize = enemyGroup.size;

          numKilled += 1;
          
          //sets the KO position temporarily
          //will be cleared in the step function if
          //a ko is not present
          koPosX_ = xnew[i];
          koPosY_ = ynew[i];
          
          switch( enemyGroup.player ){
            case(WHITE):{
              blackPrisoners_ += killSize;
              break;
            }
            case(BLACK):{
              whitePrisoners_ += killSize;
              break;
            }
            default:{
              break;
            }
          }
          clearGroup_( enemy, xnew[i], ynew[i] );
        }
      }
    }
  }

  //if a single group of size 1 is killed then we need to return that a
  //KO may be possible depending on the size of the player group
  if( numKilled == 1 && killSize == 1){
    return true;
  }
  else{
    return false;
  }
}

/*
  core function: recursively traces through every stone that is
  orthogonally connected starting at stone x,y. as it traverses it counts
  and updates the attributes Size, Liberties, and Owner, and returns them
  wrapped in a Group structure

  param x,y: coordinates of a stone within a group
  param currGroup: a copy of a Group object that holds all attributes
  
  returns currGroup after traversing all stones in the group
*/
Group GameLogic::getGroup_( int x, int y, Group currGroup ){
  //coordinates of all neighbor stones
  int xnew[] = { x-1, x, x+1, x };
  int ynew[] = { y, y-1, y, y+1 };

  //set as counted to prevent double count
  grid_[x][y]->setCounted();

  //iterate through every neighbor
  for( int i = 0; i < 4; i++ ){
    //verify we're on the board
    if( isOnBoard_( xnew[i], ynew[i] )){
      //verify we have the same player
      if( grid_[xnew[i]][ynew[i]]->isPlayer(currGroup.player)){
        //check if already counted
        if( !grid_[xnew[i]][ynew[i]]->isCounted()){
          currGroup = getGroup_( xnew[i], ynew[i], currGroup );
        }
      }
      //if it is not the same player then we have to determine if ownership
      //of the group has changed for empty territories
      else if(  currGroup.owner == EMPTY || 
                grid_[xnew[i]][ynew[i]]->isPlayer(currGroup.owner)){
        //if we have run into the same owner, or the owner hasn't been set
        //then the piece we've run into is the owner
        currGroup.owner = (Player)grid_[xnew[i]][ynew[i]]->getPlayer();
      }
      else{
        //if we've run into something different than the owner
        //then the territory is contested, once it's marked as DOMI
        //it can't be scored for either player
        currGroup.owner = (Player)DOMI;
      }
    }
  }

  //increment the group size and find any liberties next to the current stone
  currGroup.size += 1;
  currGroup.liberties += countNeighbors_( EMPTY, x, y );

  return currGroup;
}

/*
  Clears a group of stones from the board

  param player: color of the group
  param x, y: coordinates of a stone within the group
*/
void GameLogic::clearGroup_( Player player, int x, int y ) 
{
  //clear the current node
  grid_[x][y]->setPlayer( EMPTY );

  //array's for the orthogonal neighbors
  int xnew[4] = { x-1, x, x+1, x };
  int ynew[4] = { y, y-1, y, y+1 };

  //iterate through each neighbor
  for( int i = 0; i < 4; i++ ){
    //make sure we don't go outside the board
    if( isOnBoard_( xnew[i],ynew[i] )){
      //make sure the stone is of the correct player
      if( grid_[xnew[i]][ynew[i]]->isPlayer( player )){
        //clear the stone and any stones of the same player attached to it
        clearGroup_( player, xnew[i], ynew[i] );
      }
    }
  }
}

/*
  clears all of our counted marks on the board
*/
void GameLogic::clearMarks_(){
  //clear all the counted marks on the board
  for( int x = 0; x < boardSize_; x++ ){
    for( int y = 0; y < boardSize_; y++ ){
      grid_[x][y]->clearCounted();
    }
  }
}

/*
  Counts the all neighbors of a specific player around the
  provided stone.

  param target: the player type we are counting
  param x and y: the grid_ coordinates of the center stone

  return count: the number of neighbors of target color
*/
int GameLogic::countNeighbors_( Player target, int x, int y ){
  int count = 0;
  int xnew[4] = { x-1, x, x+1, x };
  int ynew[4] = { y, y-1, y, y+1 };

  for( int i = 0; i<4; i++ ){
    if( isOnBoard_( xnew[i], ynew[i] )){
      if( grid_[xnew[i]][ynew[i]]->isPlayer(target)){
        count += 1;
      }
    }
  }
  return count;
}

/*
  Scores the board according to chinese Area scoring rules
  -each stone on the board is a point for it's owning player
  -each empty group that is completely surrounded by just 1 player's
   stones and board edges has its size scored for that player.

   returns the player who has the higher score
*/
Player GameLogic::scoreArea_(){
  
  Group currGroup;

  //reset the scores
  scoreBlack_ = 0;
  scoreWhite_ = 0;
  
  //iterate through the board
  for( int x = 0; x < boardSize_; x++ ){
    for( int y = 0; y < boardSize_; y++ ){
      //reset the attributes of the group
      currGroup.size = 0;
      currGroup.liberties = 0;
      currGroup.owner = EMPTY;
      //don't double count... never double count!
      if( !grid_[x][y]->isCounted() ){
        switch( grid_[x][y]->getPlayer() ){
          //determine the player, set the player of the group
          //and determine it's size, if one of the players
          //add it to their score
          case( BLACK ):
            currGroup.player = (Player)BLACK;
            currGroup = getGroup_( x, y, currGroup );
            scoreBlack_ += currGroup.size;
            break;
          case( WHITE ):
            currGroup.player = (Player)WHITE;
            currGroup = getGroup_( x, y, currGroup );
            scoreWhite_ += currGroup.size;
            break;
          case( EMPTY ): 
            //if it's empty we have to figure out who owns the group
            currGroup.player = (Player)EMPTY;
            currGroup.owner = (Player)EMPTY;
            currGroup = getGroup_( x, y, currGroup);
            //figure out the owner of the controlled territory
            //add it to their score
            switch( currGroup.owner ){
              case( BLACK ):
                scoreBlack_ += currGroup.size;
                break;
              case( WHITE ):
                scoreWhite_ += currGroup.size;
                break;
              default:
                break;
            }     
            break;
          default:
            break;
        }
      }
    }
  }

  clearMarks_();

  //find the bigger number
  //return the winner
  if( scoreWhite_ > scoreBlack_ ){
    return WHITE;
  }
  else if( scoreBlack_ > scoreWhite_ ){
    return BLACK;
  }
  else{
    return EMPTY;
  }
}

/************************/
/************************/
/************************/
/************************/
/************************/
/************************/
/****     STONE      ****/
/************************/
/************************/
/************************/
/************************/
/************************/
/************************/

Stone::Stone( Player player, int x, int y ) 
{
  this->y = y;
  this->x = x;
  this->player = player;
  Counted = 0;
}

//group of get/set methods
void Stone::setY(int y) 
{
  this->y = y;
}

void Stone::setX(int x) 
{
  this->x = x;
}

void Stone::setPlayer( Player player ) 
{
  this->player = player;
}
  
char Stone::getPlayer() 
{
  return player;
}

bool Stone::isPlayer( Player target )
{
  if( target == player )
    {
      return true;
    }
  return false;
}

int Stone::getX() 
{
  return x;
}

int Stone::getY() 
{
  return y;
}

//set up to only allow boolean access to Counted variable
void Stone::setCounted() 
{
  Counted = true;
}

void Stone::clearCounted() 
{
  Counted = false;
}

bool Stone::isCounted() 
{
  return Counted;
}

}}  // namespaces
