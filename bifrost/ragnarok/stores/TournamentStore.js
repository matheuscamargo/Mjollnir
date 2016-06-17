import Duel from 'duel';
import GroupStage from 'groupstage';
import _ from 'underscore';
import alt from '../alt';
import TournamentActions from '../actions/TournamentActions';
import TournamentSource from '../sources/TournamentSource';

var GET_AGAIN_TIME = 500;

function toDesiredSchema(TournamentInfo, TournamentRaw) {
  var DuelTournament = TournamentRaw;
  // TODO: Add seeding;
  var OrganizeBySection = _.groupBy(DuelTournament.matches,  function(num){ return num.id.s;});
  var DesiredSchema = _.map(OrganizeBySection, function(value, key) {
                              return { id: key, rounds: _.map(_.groupBy(value, function(num) {
                                 return num.id.r;}), function(value, key) {
                                   return { id: key, matches: _.map(value, function(num) {
                                     return {id: num.id,
                                            players: [TournamentInfo.players[num.p[0] - 1], TournamentInfo.players[num.p[1] - 1]],
                                            scores: num.m};
                                   })
                                 };})
                               };});
  return {name: TournamentInfo.name, sections: DesiredSchema};
}

class TournamentStore{
  constructor(){
    this.tournamentRaw = [];
    this.tournamentInfo = [];
    this.tournament = [];
    this.errorMessage = null;

    this.bindListeners({
      handleCreate: TournamentActions.CREATE,
      handleFetch: TournamentActions.FETCH,
      handleFetchFail: TournamentActions.FETCH_FAILED,
      handlePlayMatch: TournamentActions.PLAY_MATCH,
      handlePlayMatchSuccess: TournamentActions.PLAY_MATCH_SUCCESS,
      handlePlayMatchFail: TournamentActions.PLAY_MATCH_FAILED,
      handlePlayAll: TournamentActions.PLAY_ALL,
      handleEnd: TournamentActions.END,
    });
  }

  handleCreate(TournamentInfo){
    if(TournamentInfo.type === 'single')
      this.tournamentRaw = new Duel(TournamentInfo.players.length);
    if(TournamentInfo.type === 'double')
      this.tournamentRaw = new Duel(TournamentInfo.players.length, { last: Duel.LB, short: true  });
    if(TournamentInfo.type === 'group')
      this.tournamentRaw = new GroupStage(TournamentInfo.players.length);
    this.tournamentInfo = TournamentInfo;
    console.log(this.tournamentInfo);
    this.tournament = toDesiredSchema(this.tournamentInfo, this.tournamentRaw);
  }

  handleFetch() {
    this.tournament = {};
  }

  handleFetchFail(errorMessage) {
    this.errorMessage = errorMessage;
  }

  handlePlayAll() {
    var self = this;

    function GetMatchesPromises(tournament, tournamentInfo) {
      return _.map(_.filter(tournament.matches, function(match) {
        // See if game has the players propagated and has not been scored yet
        return tournament.isPlayable(match) && (!Array.isArray(match.m));
      }), function(playableMatch) {
        return new Promise(function(resolve, reject) {
          //this.tournament = toDesiredSchema(this.tournamentInfo, this.tournamentRaw);
          //TournamentStore.emitChange();
          console.log("Jogando jogo: " + playableMatch.p);
          // TODO: change hardcoded -1 to get from actual seed
          var players = _.map(playableMatch.p, function(id) {return tournamentInfo.players[id - 1];});
          TournamentSource.playMatch({challenge: tournamentInfo.challenge, players: players}).then(function(matchInfo) {
            var tryGetResult = (matchInfo) => {
              TournamentSource.getMatch(matchInfo).then(function(matchResult) {
                if(matchResult) {
                  matchResult.p = playableMatch.p;
                  matchResult.m = _.map(players, function(p){ return p == matchResult ? 1 : 0;});
                  console.log("Fim do jogo: " + matchResult.p + ": " + matchResult.m);
                  _.extend(matchResult, {id: playableMatch.id});
                  self.handlePlayMatchSuccess(matchResult);
                  self.emitChange();
                  resolve();
                }
                else {
                  setTimeout(tryGetResult(matchInfo), GET_AGAIN_TIME);
                }
              });
            };

            tryGetResult(matchInfo);
          });
        });
      });
    }

    function RunAllMatches(tournament, tournamentInfo){
      if(tournament.isDone()){
        return;
      }
      var nextStep = Promise.all(GetMatchesPromises(tournament, tournamentInfo));
      nextStep.then(function(values) {
        RunAllMatches(tournament, tournamentInfo);
      });
    }

    RunAllMatches(this.tournamentRaw, this.tournamentInfo);
  }

  handlePlayMatch(matchInfo){
    //console.log(matchInfo);
  }

  handlePlayMatchSuccess(matchInfo) {
    //console.log(matchInfo);
    this.tournamentRaw.score(matchInfo.id, matchInfo.results);
    this.tournament = toDesiredSchema(this.tournamentInfo, this.tournamentRaw);
  }

  handlePlayMatchFail(errorMessage) {
    this.errorMessage = errorMessage;
  }

  handleEnd() {
    this.tournamentRaw = [];
    this.tournamentInfo = [];
    this.tournament = {};
  }
}

export default alt.createStore(TournamentStore, 'TournamentStore');
