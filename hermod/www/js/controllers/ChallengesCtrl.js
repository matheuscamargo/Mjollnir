controllerModule

.controller('ChallengesCtrl', function($scope, $http) {

  $http.get('js/challenges.json')
  
  .then(function(res){
    $scope.challenges = res.data;              
  });

});