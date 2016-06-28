controllerModule

.controller('GroupsCtrl', function($scope, $http, API, $state) {

  $http.get(API.url + "/api/groups", {})
  .success(function(data){
    $scope.groups = data.groups;   
  });

  $scope.join = function(groupId) {
    $http.get(API.url + "/api/join/" + groupId, {  })
    .success(function(data){
      for (var i = 0; i < $scope.groups.length; i++) {
        if ($scope.groups[i].id == groupId)
           $scope.groups[i].situation = ($scope.groups[i].situation == 'Join' ? 'Leave' : 'Join');
         }   
    });
  };

  $scope.enter = function(groupId) {
     $state.go('group.description', {id: groupId});
  };
});