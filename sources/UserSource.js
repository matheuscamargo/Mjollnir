import _ from 'underscore';

var mockData = [
  {name: 'Gustavo', ranking: 123456},
  {name: 'Peh', ranking: 5},
  {name: 'Gafa', ranking: 4},
  {name: 'Mil', ranking: 3},
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
