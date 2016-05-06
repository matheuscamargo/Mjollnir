import React from 'react';
import UserActions from '../actions/UserActions';

export default class MyComponent extends React.Component {
  constructor(props) {
      super(props);
      this._handleClick = this._handleClick.bind(this);
  }

  render() {
    return (
      <div>
        {this.props.user.name} - {this.props.user.ranking}
        <input type="button" value="Add" hidden={this.props.scores} onClick={this._handleClick}/>
      </div>);
  }

  _handleClick(e) {
    UserActions.selectUser(this.props.user);
  }
}
