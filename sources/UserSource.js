import _ from 'underscore';

var mockData = [
  {name: 'Mut', ranking: 123456},
  {name: 'Peh', ranking: 10},
  {name: 'Gafa', ranking: 8},
  {name: 'Philip', ranking: 5},
  {name: 'Bay', ranking: 3},
  {name: 'Braile', ranking: 6},
  {name: 'Josu', ranking: 7},
  {name: 'Mil', ranking: 6},
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
