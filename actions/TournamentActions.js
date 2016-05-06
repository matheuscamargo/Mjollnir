import alt from '../alt';
import TournamentSource from '../sources/TournamentSource';

class TournamentActions {
  create(tournamentInfo) {
    return tournamentInfo;
  }

  playMatch(matchInfo) {
    return matchInfo;
  }

  fetch() {
    return (dispatch) => {
      dispatch();
      TournamentSource.fetch()
        .then((tournament) => {
          this.create(tournament);
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
