function DatesInSeconds() {
  this.minute  = 60;
  this.two_mins= 120;
  this.hour    = 60*this.minute;
  this.hour_and_half = 90*this.minute;
  this.day     = 24*this.hour;
  this.week    = 7*this.day;
  this.month   = 30.5*this.day;
  this.year    = 365*this.day;
}

function getRelTime(time) {
  var dates_in_seconds = new DatesInSeconds();

  var elapsed   = Math.floor(new Date().getTime()/1000) - time;
  if( elapsed < dates_in_seconds.week )
    return getRelTimeWithinWeek(time, false);
  if (elapsed < dates_in_seconds.week*1.5)
    return 'about a week ago';
  if (elapsed < dates_in_seconds.week*3.5)
    return 'about ' + Math.round(elapsed/dates_in_seconds.week) + ' weeks ago';
  if (elapsed < dates_in_seconds.month*1.5)
    return 'about a month ago';
  return '';
  if (elapsed < dates_in_seconds.year)
    return 'about ' + Math.round(elapsed/dates_in_seconds.month) + ' months ago';
  return 'over a year ago';
}

function getProfileTime(profile_time) {
  if (typeof profile_time == "undefined")
	return "";

  var relative_time = getRelTime(profile_time);
  return relative_time ? ("Updated profile " + relative_time) : '';
  }

function getRelTimeWithinWeek(time, initialCap ) {
  var currentTime = new Date;

  var updateTime = new Date;
  updateTime.setTime(time*1000);

  var days = new Array("Sunday", "Monday", "Tuesday", "Wednesday",
                       "Thursday", "Friday", "Saturday");
  var day;

  // assumption that status messages are only shown if in the last 7 days
  if (updateTime.getDate() == currentTime.getDate()) {
    day = initialCap ? "Today" : "today";
  } else if ((updateTime.getDay() + 1) % 7 == currentTime.getDay()) {
    day = initialCap ? "Yesterday" : "yesterday";
  } else {
    day = days[updateTime.getDay()];
  }

  var hour = updateTime.getHours();
  if (hour > 11) timeOfDay = 'pm';
  else timeOfDay = 'am';
  if (hour >= 13) hour -= 12;
  if (hour == 0) hour = 12;

  var minute = updateTime.getMinutes();
  if (minute < 10) {
    minute = '0' + minute;
  }

  var tstr = day + ' at ' + hour + ':' + minute + ' ' + timeOfDay;
  return tstr;
}

function getStatusTime(status_time) {
	//return getRelTimeWithinWeek(status_time, true);
	return 'hello';
}
