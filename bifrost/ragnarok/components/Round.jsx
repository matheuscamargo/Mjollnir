import React from 'react';
import Match from './Match.jsx';
import _ from 'underscore';

export default class Round extends React.Component {
  render() {
    return (
      <div>
        Round {this.props.id}
        <table className="table">
          <tbody>
            {_.map(this.props.matches, function(match) {
              return (
                <tr key={match.id.m}>
                  <td>
                    <Match  id={match.id}
                            players={match.players}
                            scores={match.scores}>
                    </Match>
                  </td>
                </tr>
              );
            })}
          </tbody>
        </table>
      </div>);
  }
}
