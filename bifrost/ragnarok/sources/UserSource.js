import _ from 'underscore';

// var mockData = [
//   {id: 0, name: 'Mut', ranking: 123456},
//   {id: 1, name: 'Peh', ranking: 10},
//   {id: 2, name: 'Gafa', ranking: 8},
//   {id: 3, name: 'Philip', ranking: 5},
//   {id: 4, name: 'Bay', ranking: 3},
//   {id: 5, name: 'Braile', ranking: 6},
//   {id: 6, name: 'Josu', ranking: 7},
//   {id: 7, name: 'Mil', ranking: 6},
//   {id: 8, name: 'lucasmullerm', ranking: 9},
// ];


class UserSource {
  fetch() {
    var path = window.location.pathname;
    var findgid = /\/groups\/(.*)\/tournament\/new/;
    var gid = path.replace(findgid, '$1');

    return new Promise(function(resolve, reject) {
      $.ajax({
        type: "GET",
        url: "/groups/" + gid + "/users/",
        success: function(response) {
          console.log(response);
          resolve(response);
        },
      });
    });
  }
}

export default new UserSource();