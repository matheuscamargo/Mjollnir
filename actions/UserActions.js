import alt from '../alt';
import UserSource from '../sources/UserSource';

class UserActions {
  selectUser(selectedUserInfo) {
    return selectedUserInfo;
  }

  deselectUser(deselectedUserInfo) {
    return deselectedUserInfo;
  }

  selectAll() {
    return (dispatch) => {
      dispatch(); };
  }

  deselectAll() {
    return (dispatch) => {
      dispatch(); };
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
