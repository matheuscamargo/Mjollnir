import alt from '../alt';
import _ from 'underscore';

import UserActions from '../actions/UserActions';

class UserStore{
  constructor(){
    this.users = [];
    this.userErrorMessage = null;

    this.bindListeners({
      handleSelect: UserActions.SELECT_USER,
      handleDeselect: UserActions.DESELECT_USER,
      handleSelectAll: UserActions.SELECT_ALL,
      handleDeselectAll: UserActions.DESELECT_ALL,
      handleFetch: UserActions.FETCH,
      handleFetchFailed: UserActions.FETCH_FAILED,
      handleFetchSuccess: UserActions.FETCH_SUCCESS
    });
  }

  handleSelect(selectedUserInfo){
    var u = this.users.find(function(o) {return selectedUserInfo.id == o.id;});
    u.selected = true;
  }

  handleDeselect(deselectedUserInfo) {
    var u = this.users.find(function(o) {return deselectedUserInfo.id == o.id;});
    u.selected = false;
  }

  handleSelectAll() {
    this.users = _.map(this.users, function(u) {
      u.selected = true;
      return u;
    });
  }

  handleDeselectAll() {
    this.users = _.map(this.users, function(u) {
      u.selected = false;
      return u;
    });
  }

  handleFetch() {
    this.users = [];
  }

  handleFetchSuccess(usersInfo) {
    this.users = usersInfo.users;
  }

  handleFetchFailed(errorMessage) {
    this.userErrorMessage = errorMessage;
  }
}

export default alt.createStore(UserStore, 'UserStore');
