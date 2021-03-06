import _ from 'underscore';

var mockData = {
  name: 'TOURNAMENT',
  players: ["Mut", "Peh", "Gafa", "Bay", "Philip", "Josu", "Caique", "Mil"],
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

//TODO: Manage TID when integrated with DB
  playMatch(matchInfo) {
	return new Promise(function(resolve, reject) {
      // Simulating POST
      console.log(matchInfo.challenge)
      var data = {
          cid: matchInfo.challenge,
          ids: matchInfo.players,
          rounds: 1,
       	};
      $.ajax({
        type: "POST",
        url: "/tournament/1/playgame",
        data: JSON.stringify(data, null, '\t'),
        contentType: 'application/json;charset=UTF-8',
        success: function(response) {
          console.log(response);
          resolve(response);
        },
      });
    });
  }

  getMatch(matchInfo) {
    return new Promise(function(resolve, reject) {
      // Simulating GET request to find desired matchId
      $.ajax({
        type: "GET",
        url: "/tournament/1/match/" + matchInfo.mid,
        success: function(response) {
          console.log(response);
          resolve(response);
        },
        error: function() {
        	resolve(false);
        },
      });
    });
  }
}

export default new TournamentSource();
