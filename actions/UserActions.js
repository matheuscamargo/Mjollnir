import alt from '../alt';
import UserSource from '../sources/UserSource';

class UserActions {
  selectUser(selectedUserInfo) {
    return selectedUserInfo;
  }

  fetch() {
    return (dispatch) => {
      dispatch();
      UserSource.fetch()
        .then((usersInfo) => {
          this.fetchSuccess(usersInfo);
        })
        .catch((errorMessage) => {
          this.fetchFailed(errorMessage);
        });
    };
  }

  fetchSuccess(usersInfo) {
    return usersInfo;
  }

  fetchFailed(errorMessage) {
    return errorMessage;
  }
}

export default alt.createActions(UserActions);
