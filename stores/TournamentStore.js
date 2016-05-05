import alt from '../alt';
import TournamentActions from '../actions/TournamentActions';

class TournamentStore{
  constructor(){
    this.tournament = {};
    this.errorMessage = null;

    this.bindListeners({
      handleUpdate: TournamentActions.UPDATE,
      handleFetch: TournamentActions.FETCH,
      handleFetchFail: TournamentActions.FETCH_FAILED
    });
  }

  handleUpdate(tournament){
    this.tournament = tournament;
  }

  handleFetch() {
    this.tournament = {};
  }

  handleFetchFail(errorMessage) {
    this.errorMessage = errorMessage;
  }
}

export default alt.createStore(TournamentStore, 'TournamentStore');
