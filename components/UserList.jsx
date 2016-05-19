import React from 'react';
import _ from 'underscore';

import TournamentActions from '../actions/TournamentActions';
import UserActions from '../actions/UserActions';
import UserSelect from './UserSelect.jsx';

var tournamentOptions = ['single', 'double', 'group'];

export default class MyComponent extends React.Component {
  constructor(props) {
      super(props);
      this._handleClickTournament = this._handleClickTournament.bind(this);
      this._handleClickUsers = this._handleClickUsers.bind(this);
      this._handleChange = this._handleChange.bind(this);

      this.state = {typeSelected: 'single'};
  }

  render() {
    var selectedUsers = _.filter(this.props.users, function(u) {return u.selected;});
    var options = tournamentOptions.map(function(option) {
            return (
                <option key={option} value={option}>
                    {option}
                </option>
            );
        });
    return (
      <div>
        -------------USERS------------
        <input type="button" value="Add all" hidden={_.isEqual(this.props.users, selectedUsers)} onClick={this._handleClickUsers}/>
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
        <div hidden={_.isEmpty(selectedUsers)}>
          -----------SELECTED-----------
          <ul>
            {_.map(selectedUsers, function(user) {
              return (
                <li key={_.uniqueId('us_')}>
                  {user.name}
                </li>
              );
            })}
          </ul>
        </div>
        -------------------------------------
        <select className='form-control'
                value={this.state.typeSelected}
                onChange={this._handleChange}>
            {options}
        </select>
        <input type="button" value="Start Tournament" hidden={this.props.scores} onClick={this._handleClickTournament}/>
      </div>);
  }

  _handleClickTournament(e) {
    //TODO:  Add correct seeding stuff and fix this:
    TournamentActions.create({name: 'TOURNAMENT', type: this.state.typeSelected, players: _.map(_.filter(this.props.users, function(u) {return u.selected;}), function(p) {
        return p.name;
      })
    });
  }

  _handleClickUsers(e) {
    UserActions.selectAll();
  }

  _handleChange(e) {
        this.setState({typeSelected: e.target.value});
    }
}
