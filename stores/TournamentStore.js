var Duel = require('duel');
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
      handlePlayMatch: TournamentActions.PLAY_MATCH
    });
  }

  handleCreate(tournamentInfo){
    var DuelTournament = new Duel(TournamentInfo.nPlayers);
    DuelTournament.matches.forEach(function (m) {
      console.log(m.id);
    });
    console.log("Tournament: " + DuelTournament.matches);
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
