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
        resolve(_.extend({id: matchInfo}, {results: [_.random(0, 10), _.random(0, 10)]}));
      }, 250);
    });
  }
}

export default new TournamentSource();
