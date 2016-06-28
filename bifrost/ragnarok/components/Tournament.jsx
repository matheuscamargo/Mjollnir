import React from 'react';
import Results from './Results.jsx';
import _ from 'underscore';

import Section from './Section.jsx';
import TournamentActions from '../actions/TournamentActions';

export default class Tournament extends React.Component {

  render() {
    if(!this.props.id) {
      return (
        <div>
          Waiting...
        </div>
      );
    }

    if(!_.isEmpty(this.props.results)) {
      return (
        <Results  players={this.props.players} 
                  results={this.props.players}> 
        </Results>
      );
    }

    return (
      <div>
        <div>
          <h2> {this.props.id} </h2>
          <table className="table">
            <tbody>
              {_.map(this.props.sections, function(section) {
                return (
                  <tr key={section.id}>
                    <td>
                      <Section  id={section.id}
                                rounds={section.rounds}>
                      </Section>
                    </td>
                  </tr>
                );
              })}
            </tbody>
          </table>
          <input className="btn btn-default" type="button" value="Back" onClick={(e) => {TournamentActions.end();}}/>
        </div>
      </div>);
  }
}
