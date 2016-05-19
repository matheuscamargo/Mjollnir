import _ from 'underscore';

var mockData = {
  name: 'TOURNAMENT',
  players: ["Mut", "Peh", "Gafa", "Bay", "Philip", "Josu", "Caique", "Mil"],
  };

class TournamentSource {
  fetch() {
    return new Promise(function(resolve, reject) {
      setTimeout(function() {
        console.log(mockData);
        resolve(mockData);
      }, 250);
    });
  }

  playMatch(matchInfo) {
    return new Promise(function(resolve, reject) {
      setTimeout(function() {
        var x = _.random(0, 1);
        var y = 1-x;
        resolve(_.extend({id: matchInfo}, {results: [x, y]}));
      }, 500 + Math.random() * 500);
    });
  }
}

export default new TournamentSource();
