angular.module('hermod.services', [])

.service('LoginService', function($q, SERVER) {
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

.service('GameService', function($q, Games) {
    return {
        getRank: function(gameName) {
          return "not implemented";
        },

        getRankMock: function(gameName) {
          return [
          {
            position: 1,
            username: 'lucasmullerm',
            rating: '8001 ± 500'
          },
          {
            position: 2,
            username: 'gabrielilharco',
            rating: '7000 ± 500'
          }];
        }
    };
})

.factory('Games', function($http) {
  var games ;
  $http.get('js/challenges.json')
    .then(function(res){
      games  = res.data;              
  });

  return {
    all: function() {
      return challenges;
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
