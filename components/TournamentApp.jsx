import React from 'react';
import _ from 'underscore';

import TournamentStore from '../stores/TournamentStore';
import UserStore from '../stores/UserStore';
import TournamentActions from '../actions/TournamentActions';
import UserActions from '../actions/UserActions';
import connectToStores from 'alt-utils/lib/connectToStores';

import Tournament from './Tournament.jsx';
import UserList from './UserList.jsx';

class TournamentApp extends React.Component {

  static getStores(props) {
    return [TournamentStore, UserStore];
  }

  static getPropsFromStores() {
    return _.extend(TournamentStore.getState(), UserStore.getState());
  }

  static componentDidConnect() {
    TournamentActions.fetch();
    UserActions.fetch();
  }

  render() {
    if(this.props.errorMessage) {
      console.log(this.props.errorMessage);
      return (
        <div> Something is wrong: {this.props.errorMessage.toString()}</div>
      );
    }

    if(!this.props.tournament.name) {
      return (
        <div>
          Loading...
        </div>
      );
    }

    return (
      <div>
        <UserList users={this.props.users} selected={this.props.selectedUsers}>
        </UserList>
        <Tournament id={this.props.tournament.name}
                    sections={this.props.tournament.sections}>
        </Tournament>
      </div>
    );
  }
}

export default connectToStores(TournamentApp);
