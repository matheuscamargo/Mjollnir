import React from 'react';
import _ from 'underscore';

import TournamentActions from '../actions/TournamentActions';
import UserActions from '../actions/UserActions';
import UserAction from './UserAction.jsx';

var tournamentOptions = {
  types: ['single', 'double', 'group'],
  challenges: ['61dd3230-2ea1-4cc1-b521-457f91b03a9e'],
};

export default class MyComponent extends React.Component {
  constructor(props) {
      super(props);
      this._handleClickTournament = this._handleClickTournament.bind(this);
      this._handleTypeChange = this._handleTypeChange.bind(this);
      this._handleChallengeChange = this._handleChallengeChange.bind(this);

      this.state = {typeSelected: 'single', challengeSelected: 'ttt'};
  }

  render() {
    var selectedUsers = _.filter(this.props.users, function(u) {return u.selected;});
    var typeOptions = tournamentOptions.types.map(function(option) {
            return (
                <option key={option} value={option}>{option}</option>
            );
        });
    var challengeOptions = tournamentOptions.challenges.map(function(option) {
            return (
                <option key={option} value={option}>{option}</option>
            );
        });

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
                          Ranking
                      </th>
                      <th>
                        <input className="btn btn-default" type="button" value="Add all" disabled={_.isEqual(this.props.users, selectedUsers)} onClick={(u) => {UserActions.selectAll();}}/>
                      </th>
                    </tr>
                  </thead>
                  <tbody>
                    {_.map(this.props.users, function(user) {
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
                          Ranking
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
            Tournament Type:
            <select className='form-control'
                    value={this.state.challengeSelected}
                    onChange={this._handleChallengeChange}>
                {challengeOptions}
            </select>
          </div>
          <div>
            <input type="button" className="btn btn-default" value="Start Tournament" hidden={this.props.scores} onClick={this._handleClickTournament}/>
          </div>
        </div>
      </div>);
  }

  _handleClickTournament(e) {
    //TODO:  Add correct seeding stuff and fix this:
    TournamentActions.create({name: 'TOURNAMENT', type: this.state.typeSelected, challenge:this.state.challengeSelected, players: _.map(_.filter(this.props.users, function(u) {return u.selected;}), function(p) {
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
