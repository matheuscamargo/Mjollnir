import alt from '../alt';
import _ from 'underscore';

import UserActions from '../actions/UserActions';

class UserStore{
  constructor(){
    this.users = [];
    this.selectedUsers = [];
    this.userErrorMessage = null;

    this.bindListeners({
      handleSelect: UserActions.SELECT_USER,
      handleFetch: UserActions.FETCH,
      handleFetchFailed: UserActions.FETCH_FAILED,
      handleFetchSuccess: UserActions.FETCH_SUCCESS
    });
  }

  handleSelect(selectedUserInfo){
    if(_.contains(this.selectedUsers, selectedUserInfo)) return;
    this.selectedUsers.push(selectedUserInfo);
  }

  handleFetch() {
    this.users = [];
  }

  handleFetchSuccess(usersInfo) {
    this.users = usersInfo;
  }

  handleFetchFailed(errorMessage) {
    this.userErrorMessage = errorMessage;
  }
}

export default alt.createStore(UserStore, 'UserStore');
