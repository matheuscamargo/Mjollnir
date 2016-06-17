import React from 'react';
import TournamentActions from '../actions/TournamentActions';

export default class MyComponent extends React.Component {
  constructor(props) {
        super(props);
        this._handleClick = this._handleClick.bind(this);
    }

  render() {
    // TODO: Does any game support more than 2 players?
    return (
      <div>
        <table className="table">
          <thead>
            <tr>
              <td>
                Match {this.props.id.m}
              </td>
              <td>
                <input type="button" className="btn btn-default" value="Play" disabled={this.props.scores || (!this.props.players[0] || !this.props.players[1])} onClick={this._handleClick}/>
              </td>
            </tr>
          </thead>
          <tbody>
            <tr>
              <td>
                {this.props.players[0] ? this.props.players[0] : '-'}
              </td>
              <td>
                {this.props.scores ? this.props.scores[0] : '-'}
              </td>
            </tr>
            <tr>
              <td>
                {this.props.players[1] ? this.props.players[1] : '-'}
              </td>
              <td>
                {this.props.scores ? this.props.scores[1] : '-'}
              </td>
            </tr>
          </tbody>
        </table>
      </div>);
  }

  _handleClick(e) {
    TournamentActions.playMatch(this.props.id);
  }
}
