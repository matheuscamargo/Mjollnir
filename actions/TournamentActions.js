import alt from '../alt';
import TournamentSource from '../sources/TournamentSource';

class TournamentActions {
  update(TournamentInfo) {
    return TournamentInfo;
  }

  fetch() {
    return (dispatch) => {
      dispatch();
      TournamentSource.fetch()
        .then((tournament) => {
          this.update(tournament);
        })
        .catch((errorMessage) => {
          this.fetchFailed(errorMessage);
        });
    };
  }

  fetchFailed(errorMessage) {
    return errorMessage;
  }
}

export default alt.createActions(TournamentActions);
