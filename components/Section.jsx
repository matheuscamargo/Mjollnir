import React from 'react';
import Round from './Round.jsx';
import _ from 'underscore';

export default class MyComponent extends React.Component {
  render() {
    return (
      <div>
        Section {this.props.id}
        <table>
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
