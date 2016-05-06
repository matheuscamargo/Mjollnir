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
      handlePlayMatchSuccess: TournamentActions.PLAY_MATCH_SUCCESS,
      handlePlayMatchFail: TournamentActions.PLAY_MATCH_FAILED
    });
  }

  handleCreate(tournamentInfo){
    this.tournament = tournamentInfo;
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
  }

  handlePlayMatchFail(errorMessage) {
    this.errorMessage = errorMessage;
  }
}

export default alt.createStore(TournamentStore, 'TournamentStore');
