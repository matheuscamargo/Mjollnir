import alt from '../alt';
import _ from 'underscore';

import UserActions from '../actions/UserActions';

class UserStore{
  constructor(){
    this.users = [];
    this.userErrorMessage = null;

    this.bindListeners({
      handleSelect: UserActions.SELECT_USER,
      handleSelectAll: UserActions.SELECT_ALL,
      handleFetch: UserActions.FETCH,
      handleFetchFailed: UserActions.FETCH_FAILED,
      handleFetchSuccess: UserActions.FETCH_SUCCESS
    });
  }

  handleSelect(selectedUserInfo){
    var u = this.users.find(function(o) {return selectedUserInfo.id == o.id;});
    u.selected = true;
  }

  handleSelectAll() {
    this.users = _.map(this.users, function(u) {
      u.selected = true;
      return u;
    });
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
