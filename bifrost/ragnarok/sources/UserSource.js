import _ from 'underscore';

var mockData = [
  {id: 0, name: 'Mut', ranking: 123456},
  {id: 1, name: 'victorvpascoal', ranking: 10},
  {id: 2, name: 'Gafa', ranking: 8},
  {id: 3, name: 'Philip', ranking: 5},
  {id: 4, name: 'Bay', ranking: 3},
  {id: 5, name: 'Braile', ranking: 6},
  {id: 6, name: 'Josu', ranking: 7},
  {id: 7, name: 'Mut', ranking: 6},
  {id: 8, name: 'victorvpascoal', ranking: 9},
];


class UserSource {
  fetch() {
    return new Promise(function(resolve, reject) {
      setTimeout(function() {
        console.log(mockData);
        resolve(mockData);
      }, 250);
    });
  }
}

export default new UserSource();
