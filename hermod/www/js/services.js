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
    image: 'img/gamao.jpg',
    description: 'jogo do gamao'
  }, {
    id: 1,
    name: 'Tic Tac Toe',
    image: 'img/tictactoe.png',
    description: 'jogo da velha'
  }, {
    id: 2,
    name: 'Tron',
    image: 'img/tron.png',
    description: 'aka snake fight'
  }, {
    id: 3,
    name: 'Wumpus',
    image: 'img/wumpus.png',
    description: 'jogo que nao conheço'
  }, {
    id: 4,
    name: 'Go',
    image: 'img/go.png',
    description: 'xadrez da china'
  }, {
    id: 5,
    name: 'Xadrez',
    image: 'img/chess.jpg',
    description: 'xadrez nao da china'
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
