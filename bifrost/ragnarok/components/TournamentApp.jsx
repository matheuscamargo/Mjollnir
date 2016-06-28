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
    UserActions.fetch();
  }

  render() {
    if(this.props.errorMessage) {
      console.log(this.props.errorMessage);
      return (
        <div> Something is wrong: {this.props.errorMessage.toString()}</div>
      );
    }

    if(!_.isEmpty(this.props.tournament)) {
      return (
        <Tournament id={this.props.tournament.name}
                    sections={this.props.tournament.sections}
                    players={this.props.tournamentInfo.players}
                    results={this.props.results}>
        </Tournament>
      );
    }
    else if(!_.isEmpty(this.props.users)) {
      return (
        <UserList users={this.props.users} selected={this.props.selectedUsers} hidden={this.props.tournament.name}>
        </UserList>
      );
    }
    else {
      return (
        <div>
          <h1> New Tournament </h1> 
          <div> 
            Loading... 
          </div>
        </div>
      );
    }
  }
}

export default connectToStores(TournamentApp);
