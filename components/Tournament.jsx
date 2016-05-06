import React from 'react';
import Section from './Section.jsx';
import _ from 'underscore';

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
          <table>
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
