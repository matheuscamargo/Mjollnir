var Duel = require('duel');
var _ = require('underscore');
import alt from '../alt';
import TournamentActions from '../actions/TournamentActions';

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
      handlePlayMatchFail: TournamentActions.PLAY_MATCH_FAILED
    });
  }

  handleCreate(TournamentInfo){
    this.tournamentRaw = new Duel(TournamentInfo.players.length);
    this.tournamentInfo = TournamentInfo;
    this.tournament = toDesiredSchema(this.tournamentInfo, this.tournamentRaw);
  }

  handleFetch() {
    this.tournament = {};
  }

  handleFetchFail(errorMessage) {
    this.errorMessage = errorMessage;
  }

  handlePlayMatch(matchInfo){
    console.log("Playing Match:");
    console.log(matchInfo);
  }

  handlePlayMatchSuccess(matchInfo) {
    console.log("Match Success");
    console.log(matchInfo);
    this.tournamentRaw.score(matchInfo.id, matchInfo.results);
    this.tournament = toDesiredSchema(this.tournamentInfo, this.tournamentRaw);
  }

  handlePlayMatchFail(errorMessage) {
    this.errorMessage = errorMessage;
  }
}

export default alt.createStore(TournamentStore, 'TournamentStore');
