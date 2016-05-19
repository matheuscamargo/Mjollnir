import React from 'react';
import _ from 'underscore';

import TournamentActions from '../actions/TournamentActions';
import UserSelect from './UserSelect.jsx';

var tournamentOptions = ['single', 'double', 'group'];

export default class MyComponent extends React.Component {
  constructor(props) {
      super(props);
      this._handleClick = this._handleClick.bind(this);
      this._handleChange = this._handleChange.bind(this);

      this.state = {typeSelected: 'duel'};
  }

  render() {

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
        <select className='form-control'
                value={this.state.typeSelected}
                onChange={this._handleChange}>
            {options}
        </select>
        <input type="button" value="Start Tournament" hidden={this.props.scores} onClick={this._handleClick}/>
      </div>);
  }

  _handleClick(e) {
    //TODO:  Add correct seeding stuff and fix this:
    TournamentActions.create({name: 'TOURNAMENT', type: this.state.typeSelected, players: _.map(this.props.selected, function(p) {
        return p.name;
      })
    });
  }

  _handleChange(e) {
        this.setState({typeSelected: e.target.value});
    }
}
