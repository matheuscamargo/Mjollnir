import React from 'react';
import _ from 'underscore';

import TournamentActions from '../actions/TournamentActions';
import UserActions from '../actions/UserActions';
import UserAction from './UserAction.jsx';

var MIN_USERS = 4;
// TODO Get a dictonary from server for each of the possible games.
var tournamentOptions = {
  types: {
    "single": "Single Elimination",
    "double": "Double Elimination",
    "group":  "Group Tournament"
  },
  challenges: {
    "4f50c959-700b-4570-ae58-54592b4d316c":  "Snake Fight",
    "61dd3230-2ea1-4cc1-b521-457f91b03a9e":  "Tic-Tac-Toe",
    "c7587b14-6ed4-4c4f-9155-47f869137916":  "Backgammon",
  },
};

export default class MyComponent extends React.Component {
  constructor(props) {
      super(props);
      this._handleClickTournament = this._handleClickTournament.bind(this);
      this._handleTypeChange = this._handleTypeChange.bind(this);
      this._handleChallengeChange = this._handleChallengeChange.bind(this);

      this.state = {typeSelected: _.keys(tournamentOptions.types)[0], challengeSelected: _.keys(tournamentOptions.challenges)[0]};
  }

  render() {
    var self = this;
    var selectableUsers = _.select(this.props.users, function(u) {return _.contains(u.subs, self.state.challengeSelected)});
    var selectedUsers = _.filter(selectableUsers, function(u) {return u.selected;});

    var typeOptions = _.values(_.mapObject(tournamentOptions.types, function(value, key) {
              return (
                  <option key={key} value={key}>{value}</option>
              );
          }));
    var challengeOptions = _.values(_.mapObject(tournamentOptions.challenges, function(value, key) {
              return (
                  <option key={key} value={key}>{value}</option>
              );
          }));

        //TODO: Break into better looking components?
    return (
      <div>
        <table className="table">
          <thead>
            <tr>
              <th>
                USERS
              </th>
              <th>
                SELECTED
              </th>
            </tr>
          </thead>
          <tbody>
            <tr>
              <td>
                <table className="table">
                  <thead>
                    <tr>
                      <th>
                          Name
                      </th>
                      <th>
                        <input className="btn btn-default" type="button" value="Add all" disabled={_.isEqual(selectableUsers, selectedUsers)} onClick={(u) => {UserActions.selectAll();}}/>
                      </th>
                    </tr>
                  </thead>
                  <tbody>
                    {_.map(selectableUsers, function(user) {
                      return (
                        <UserAction key={_.uniqueId('u_')} user={user} text="Add" action={UserActions.selectUser} btnCondition={user.selected}>
                        </UserAction>
                      );
                    })}
                  </tbody>
                </table>
              </td>
              <td>
                <table className="table">
                  <thead>
                    <tr>
                      <th>
                          Name
                      </th>
                      <th>
                        <input className="btn btn-default" type="button" value="Remove all" disabled={_.isEmpty(selectedUsers)} onClick={(u) => {UserActions.deselectAll();}}/>
                      </th>
                    </tr>
                  </thead>
                  <tbody>
                    {_.map(selectedUsers, function(user) {
                      return (
                        <UserAction key={_.uniqueId('us_')} user={user} text="Remove" action={UserActions.deselectUser}>
                        </UserAction>
                      );
                    })}
                  </tbody>
                </table>
              </td>
            </tr>
          </tbody>
          <tfoot>
          <tr>
              <th>
                <div color="red" hidden={selectableUsers.lenght > 0}>
                  There are no users with submissions for the selected game.
                </div>
              </th>
              <th>
              </th>
            </tr>
          </tfoot>
        </table>
        <div className="col-md-6 col-md-offset-3">
          <div>
            Tournament Type:
            <select className='form-control'
                    value={this.state.typeSelected}
                    onChange={this._handleTypeChange}>
                {typeOptions}
            </select>
          </div>
          <div>
            Tournament Game:
            <select className='form-control'
                    value={this.state.challengeSelected}
                    onChange={this._handleChallengeChange}>
                {challengeOptions}
            </select>
          </div>
          <br></br>
          <div>
            <span hidden={selectedUsers.length < MIN_USERS}> 
              <input type="button" className="btn btn-default" value="Start Tournament"  onClick={(e) => this._handleClickTournament(e, selectedUsers)}/>
            </span>
            <span color="red" hidden={!(selectedUsers.length < MIN_USERS)}> 
              The minimum number of participants is {MIN_USERS}. 
            </span>
          </div>
        </div>
      </div>);
  }

  _handleClickTournament(e, selected) {
    //TODO:  Add correct seeding stuff and fix this:
    TournamentActions.create({name: 'TOURNAMENT', type: this.state.typeSelected, challenge:this.state.challengeSelected, players: _.map(selected, function(p) {
        return p.name;
      })
    });
  }

  _handleTypeChange(e) {
    this.setState({typeSelected: e.target.value});
  }

  _handleChallengeChange(e) {
    this.setState({challengeSelected: e.target.value});
  }
}