import React from 'react';
import _ from 'underscore';

export default class Results extends React.Component {
  render() {
    var self = this;

    var orderedResults = _.sortBy(this.props.results, "pos");    
    var resultRender = _.map(orderedResults, function(r) {
      return (
        <tr key={_.uniqueId("res_")}>
          <td>
            {r.pos}
          </td>
          <td>
            {self.props.players[r.seed - 1]}
          </td>
        </tr>
      );
    });

    return (
      <div>
        <h1> Tournament Results </h1>
        <table className="table">
          <thead>
            <tr>
              <td>
                 Position
              </td>
              <td>
                Player
              </td>
            </tr>
          </thead>
          <tbody>
            {resultRender}
          </tbody>
        </table>
      </div>);
  }
}
