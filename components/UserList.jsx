import React from 'react';
import _ from 'underscore';

import TournamentActions from '../actions/TournamentActions';

import UserSelect from './UserSelect.jsx';

export default class MyComponent extends React.Component {
  constructor(props) {
      super(props);
      this._handleClick = this._handleClick.bind(this);
  }

  render() {
    return (
      <div>
        -------------USERS------------
        <ul>
          {_.map(this.props.users, function(user) {
            return (
              <li key={_.uniqueId('u_')}>
                <UserSelect user={user}>
                </UserSelect>
              </li>
            );
          })}
        </ul>
        <div hidden={_.isEmpty(this.props.selected)}>
          -----------SELECTED-----------
          <ul>
            {_.map(this.props.selected, function(user) {
              return (
                <li key={_.uniqueId('us_')}>
                  {user.name}
                </li>
              );
            })}
          </ul>
        </div>
        -------------------------------------
        <input type="button" value="Start Tournament" hidden={this.props.scores} onClick={this._handleClick}/>
      </div>);
  }

  _handleClick(e) {
    //TODO:  Add correct seeding stuff and fix this:
    TournamentActions.create({name: 'TOURNAMENT', players: _.map(this.props.selected, function(p) {
        return p.name;
      })
    });
  }
}
