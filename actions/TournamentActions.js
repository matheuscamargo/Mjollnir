import alt from '../alt';
import TournamentSource from '../sources/TournamentSource';

class TournamentActions {
  create(tournamentInfo) {
    return tournamentInfo;
  }

  playMatch(matchInfo) {
    return (dispatch) => {
      dispatch(matchInfo);
      TournamentSource.playMatch(matchInfo)
        .then((tournament) => {
          this.playMatchSuccess(tournament);
        })
        .catch((errorMessage) => {
          this.playMatchFailed(errorMessage);
        });
    };
  }

  playMatchSuccess(matchInfo) {
    return matchInfo;
  }

  playMatchFailed(errorMessage) {
    return errorMessage;
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
