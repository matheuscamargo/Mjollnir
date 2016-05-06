import _ from 'underscore';

var mockData = {
  name: 'TEST',
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
        resolve(_.extend(matchInfo, {result: [1, 0]}));
      }, 250);
    });
  }
}

export default new TournamentSource();
