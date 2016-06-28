servicesModule


// Mock for the challenges info

.service('ChallengesMock', function($q) {
    return {
        getRank: function(challengeName) {
          return [
          {
            sequence: 1,
            username: 'Mock user 1',
            rating: '8001 ± 500'
          },
          {
            sequence: 2,
            username: 'Mock user 2',
            rating: '7000 ± 500'
          }];
        }
    };
});