controllerModule

.controller('GroupTournamentsCtrl', function ($scope, $stateParams, $rootScope, $http, API) {

  $http.get(API.url + "/api/group/" + $stateParams.id, {})
  .success(function(data){
    $scope.group = data;
  });

});