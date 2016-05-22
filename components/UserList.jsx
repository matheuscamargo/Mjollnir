import React from 'react';
import _ from 'underscore';

import TournamentActions from '../actions/TournamentActions';
import UserActions from '../actions/UserActions';
import UserAction from './UserAction.jsx';

var tournamentOptions = ['single', 'double', 'group'];

export default class MyComponent extends React.Component {
  constructor(props) {
      super(props);
      this._handleClickTournament = this._handleClickTournament.bind(this);
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
                        <tr key={_.uniqueId('u_')}>
                          <UserAction user={user} text="Add" action={UserActions.selectUser} btnCondition={user.selected}>
                          </UserAction>
                        </tr>
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
                        <tr key={_.uniqueId('us_')}>
                          <UserAction user={user} text="Remove" action={UserActions.deselectUser}>
                          </UserAction>
                        </tr>
                      );
                    })}
                  </tbody>
                </table>
              </td>
            </tr>
          </tbody>
        </table>
        <div className="col-md-6 col-md-offset-3">
          Tournament Type:
          <select className='form-control'
                  value={this.state.typeSelected}
                  onChange={this._handleChange}>
              {options}
          </select>
          <input type="button" value="Start Tournament" hidden={this.props.scores} onClick={this._handleClickTournament}/>
        </div>
      </div>);
  }

  _handleClickTournament(e) {
    //TODO:  Add correct seeding stuff and fix this:
    TournamentActions.create({name: 'TOURNAMENT', type: this.state.typeSelected, players: _.map(_.filter(this.props.users, function(u) {return u.selected;}), function(p) {
        return p.name;
      })
    });
  }

  _handleChange(e) {
        this.setState({typeSelected: e.target.value});
    }
}
