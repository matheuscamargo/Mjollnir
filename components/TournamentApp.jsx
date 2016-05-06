import React from 'react';
//import _ from 'lodash';
import TournamentStore from '../stores/TournamentStore';
import TournamentActions from '../actions/TournamentActions';
import connectToStores from 'alt-utils/lib/connectToStores';

import Tournament from './Tournament.jsx';


class TournamentApp extends React.Component {

  static getStores(props) {
    return [TournamentStore];
  }

  static getPropsFromStores() {
    return TournamentStore.getState();
  }

  static componentDidConnect() {
    TournamentActions.fetch();
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
        <Tournament id={this.props.tournament.name}
                    sections={this.props.tournament.sections}>
        </Tournament>
      </div>
    );
  }
}

export default connectToStores(TournamentApp);
