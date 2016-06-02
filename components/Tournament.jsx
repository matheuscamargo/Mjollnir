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
          {this.props.id}
          <input className="btn btn-default" type="button" value="Play" onClick={(e) => {TournamentActions.playAll();}}/>
          <input className="btn btn-default" type="button" value="End" onClick={(e) => {TournamentActions.end();}}/>
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
        </div>
      </div>);
  }
}
