import React from 'react';
import Results from './Results.jsx';
import _ from 'underscore';

import Section from './Section.jsx';
import TournamentActions from '../actions/TournamentActions';

export default class Tournament extends React.Component {
  constructor(props) {
      super(props);
      this._handleClickBrackets = this._handleClickBrackets.bind(this);
      this._handleClickResults = this._handleClickResults.bind(this);

      this.state = {seeResults: false};
  }

  render() {
    if(!this.props.id) {
      return (
        <div>
          Waiting...
        </div>
      );
    }

    if(this.state.seeResults) {
      return (
        <div>
          <Results  players={this.props.players} 
                    results={this.props.results}> 
          </Results>
          <input className="btn btn-default" type="button" hidden={_.isEmpty(this.props.results)} value="Back to Brackets!" onClick={this._handleClickBrackets}/>
        </div>
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
          <div>
            <input className="btn btn-default" type="button" value="Back" onClick={(e) => {TournamentActions.end();}}/>
          </div>
          <div hidden={_.isEmpty(this.props.results)}>        
            <input className="btn btn-default pull-right" type="button" value="Go to Results!" onClick={this._handleClickResults}/>
          </div>
        </div>
      </div>);
  }

  _handleClickResults(e) {
    this.setState({seeResults: true});
  }

  _handleClickBrackets(e) {
    this.setState({seeResults: false});
  }

}
