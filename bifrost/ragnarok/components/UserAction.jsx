import React from 'react';
import UserActions from '../actions/UserActions';

export default class UserAction extends React.Component {
  constructor(props) {
      super(props);
      this._handleClick = this._handleClick.bind(this);
  }

  render() {
    return (
      <tr>
        <td>
          {this.props.user.name}
        </td>
        <td>
          <input className="btn btn-default" type="button" value={this.props.text} disabled={this.props.btnCondition} onClick={this._handleClick}/>
        </td>
      </tr>
    );
  }

  _handleClick(e) {
    this.props.action(this.props.user);
  }
}
