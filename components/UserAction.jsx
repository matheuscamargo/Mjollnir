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
        <td>
          {this.props.user.name}
        </td>
        <td>
          {this.props.user.ranking}
        </td>
        <td>
          <input type="button" value={this.props.text} hidden={this.props.btnCondition} onClick={this._handleClick}/>
        </td>
      </div>);
  }

  _handleClick(e) {
    this.props.action(this.props.user);
  }
}
