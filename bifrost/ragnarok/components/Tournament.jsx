import React from 'react';
import _ from 'underscore';

import Section from './Section.jsx';
import TournamentActions from '../actions/TournamentActions';

export default class MyComponent extends React.Component {

  render() {
    if(!this.props.id) {
      return (
        <div>
          Waiting...
        </div>
      );
    }

    return (
      <div>
        <div>
          <h2> {this.props.id} </h2>
          <div> <input className="btn btn-primary pull-right" type="button" value="Play Tournament" onClick={(e) => {TournamentActions.playAll();}}/> </div>
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
