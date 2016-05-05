angular.module('hermod.services', [])

// Service to handle the check of credentials 

.service('LoginService', function($q, SERVER) {
    return {
        loginUser: function(name, pw) {
            var deferred = $q.defer();
            var promise = deferred.promise;
            
            // TODO: login request

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

.service('ChallengesService', function($http, $q, SERVER) {
    return {
        getRank: function(challengeName) {
            $http({
                url: SERVER['url'] + "api/challenge/" + challengeName,
            }).
            success(function(data) {
                console.log(data['rank']);
                return data['rank'];
            });
        }
    };
})

// Mock for the challenges info

.service('ChallengesMock', function($q) {
    return {
        getRank: function(challengeName) {
          return [
          {
            sequence: 1,
            username: 'lucasmullerm',
            rating: '8001 ± 500'
          },
          {
            sequence: 2,
            username: 'gabrielilharco',
            rating: '7000 ± 500'
          }];
        }
    };
});


