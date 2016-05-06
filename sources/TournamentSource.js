import _ from 'underscore';

var mockData = {
  name: "Test Tournament",
  sections: [
    {
      id: 1,
      rounds: [
        {
          id: 1,
          matches: [
            {
              id: {s: 1, r: 1, m: 1},
              players: ['Mut', 'Peh'],
              scores: null,
            },
            {
              id: {s: 1, r: 1, m: 2},
              players: ['Gafa', 'Baya'],
              scores: null,
            },
          ],
        },
        {
          id: 2,
          matches: [
            {
              id: {s: 1, r: 2, m: 1},
              players: ['Mut', 'Peh'],
              scores: null,
            },
            {
              id: {s: 1, r: 2, m: 2},
              players: ['Gafa', 'Baya'],
              scores: null,
            },
          ],
        },
      ],
    }
  ],
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
