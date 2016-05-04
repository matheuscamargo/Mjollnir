angular.module('starter.services', [])

.service('LoginService', function($q) {
    return {
        loginUser: function(name, pw) {
            var deferred = $q.defer();
            var promise = deferred.promise;
            
            // login request

            if (true) {
                deferred.resolve('Welcome ' + name + '!');
            } else {
                deferred.reject('Wrong credentials.');
            }
            promise.success = function(fn) {
                promise.then(fn);
                return promise;
            };
            promise.error = function(fn) {
                promise.then(null, fn);
                return promise;
            };
            return promise;
        }
    };
})

.factory('Games', function() {
  // Might use a resource here that returns a JSON array

  // Some fake testing data
  var games = [{
    id: 0,
    name: 'Gamão',
    face: 'img/gamao.png',
    description: 'jogo do gamao'
  }, {
    id: 1,
    name: 'Tic Tac Toe',
    face: 'img/tictactoe.png',
    description: 'jogo da velha'
  }, {
    id: 2,
    name: 'Tron',
    face: 'img/tron.png',
    description: 'aka snake fight'
  }, {
    id: 3,
    name: 'Wumpus',
    face: 'img/wumpus.png',
    description: 'jogo que nao conheço'
  }];

  return {
    all: function() {
      return games;
    },
    remove: function(game) {
      games.splice(games.indexOf(game), 1);
    },
    get: function(gameId) {
      for (var i = 0; i < games.length; i++) {
        if (games[i].id === parseInt(gameId)) {
          return games[i];
        }
      }
      return null;
    }
  };
});
