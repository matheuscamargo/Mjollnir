var Duel = require('duel');
var _ = require('underscore');
import alt from '../alt';
import TournamentActions from '../actions/TournamentActions';



class TournamentStore{
  constructor(){
    this.tournament = {};
    this.errorMessage = null;

    this.bindListeners({
      handleCreate: TournamentActions.CREATE,
      handleFetch: TournamentActions.FETCH,
      handleFetchFail: TournamentActions.FETCH_FAILED,
      handlePlayMatch: TournamentActions.PLAY_MATCH,
    });
  }

  handleCreate(TournamentInfo){
    var DuelTournament = new Duel(TournamentInfo.Players.length);
    DuelTournament.matches.forEach(function (m) {
      console.log(m.id);
    });

    var OrganizeBySection = _.groupBy(DuelTournament.matches,  function(num){ return num.id.s;});
    var DesiredSchema = _.map(OrganizeBySection, function(value, key)
                        { return { id: key, rounds: _.map(_.groupBy(value, function(num){ return num.id.r;}), function(value, key)
                        { return { id: key, matches: _.map(value, function(num){return {id: num.id, players: [TournamentInfo.Players[num.p[0] - 1], TournamentInfo.Players[num.p[1] - 1]]}})}})}});

    console.log(DesiredSchema);

  }

  handleFetch() {
    this.tournament = {};
  }

  handleFetchFail(errorMessage) {
    this.errorMessage = errorMessage;
  }

  handlePlayMatch(matchInfo){
    console.log(matchInfo);
  }

}

export default alt.createStore(TournamentStore, 'TournamentStore');
