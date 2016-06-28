import React from 'react';
import Round from './Round.jsx';
import _ from 'underscore';

export default class Section extends React.Component {
  render() {
    return (
      <div>
        Section {this.props.id}
        <table className="table">
          <tbody>
            <tr>
              {_.map(this.props.rounds, function(round) {
                return (
                  <td key={round.id}>
                    <Round  id={round.id}
                            matches={round.matches}>
                    </Round>
                  </td>
                );
              })}
            </tr>
          </tbody>
        </table>
      </div>);
  }
}
