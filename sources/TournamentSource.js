var mockData = {
  name: "Test Tournament",
  sections: [
    {
      id: 1,
      rounds: [
        {
          id: 1,
          mathes: [
            {
              id: 1,
              players: ['Mut', 'Peh'],
              scores: [1, 0],
            },
            {
              id: 2,
              players: ['Gafa', 'Baya'],
              scores: [1, 0],
            },
          ],
        },
        {
          id: 2,
          mathes: [
            {
              id: 1,
              players: ['Mut', 'Peh'],
              scores: [1, 0],
            },
            {
              id: 2,
              players: ['Gafa', 'Baya'],
              scores: [1, 0],
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
}

export default new TournamentSource();
