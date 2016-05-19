import Duel from 'duel';
import GroupStage from 'groupstage';
import _ from 'underscore';
import alt from '../alt';
import TournamentActions from '../actions/TournamentActions';
import TournamentSource from '../sources/TournamentSource';

function toDesiredSchema(TournamentInfo, TournamentRaw) {
  var DuelTournament = TournamentRaw;

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

    function GetMatchesPromises(tournament) {
      return _.map(_.filter(tournament.matches, function(match) {
        return tournament.isPlayable(match);
      }), function(playableMatch) {
        return new Promise(function(resolve, reject) {
          //this.tournament = toDesiredSchema(this.tournamentInfo, this.tournamentRaw);
          //TournamentStore.emitChange();
          console.log("Jogando jogo: " + playableMatch.p);
          TournamentSource.playMatch(playableMatch.id).then(function(matchResult) {
            console.log("Fim do jogo: " + matchResult.p + ": " + matchResult.m);
            self.handlePlayMatchSuccess(matchResult);
            self.emitChange();
            resolve();
          });
        });
      });
    }

    function RunAllMatches(tournament){
      if(tournament.isDone()){
        return;
      }
      var nextStep = Promise.all(GetMatchesPromises(tournament));
      nextStep.then(function(values) {
        RunAllMatches(tournament);
      });
    }

    RunAllMatches(this.tournamentRaw);
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
}

export default alt.createStore(TournamentStore, 'TournamentStore');
