
function user() {
	var s={
		"user":"",
		"token":""
	};
	function readStorage(){
		s.user	=localStorage.getItem("TanTa_user_name");
		s.token	=localStorage.getItem("TanTa_user_token");
	}
	function saveStorage(){
		localStorage.setItem("TanTa_user_name"		,s.user);
		localStorage.setItem("TanTa_user_token"	,s.token);
	}
	s.logout=function () {
		s.user="";
		s.token="";
		saveStorage();
	};
	s.set=function (u,t) {
		s.user=u;
		s.token=t;
		saveStorage();
	};
	s.login=function (u,p,callback) {
		$.post(
			"/api/login",
			{
				"user" : u,
				"pwd"  : p
			},
			function (res) {
				if(res=="noarg!"){
					alert("参数异常");
				}else
				if(res=="fail!"){
					alert("密码错误");
				}else{
					alert("登录成功");
					s.set(u,res);
					if(callback)
						callback();
				}
			}
		);
	};
	s.isloged=function (callback) {
		if(s.user && s.token){
			$.post(
				"/api/isloged",
				{
					"user" : s.user,
					"token": s.token
				},
				function (res) {
					if(res=="1")
						callback(true);
					else
						callback(false);
				}
			);
		}else
			if(callback)
				callback(false);
	};
	s.setPwd=function (pwd,callback) {
		$.post(
			"/api/changepwd",
			{
				"user" : s.user,
				"token": s.token,
				"pwd"  : pwd
			},
			function (res) {
				if(res=="noarg!"){
					alert("参数异常");
				}else
				if(res=="nologin!"){
					alert("未登录");
				}else
				if(res=="ok!"){
					alert("设置成功");
					if(callback)
						callback();
				}
			}
		);
	};
	readStorage();
	return s;
}

function sendPassage() {
	var s=user();
	s.send=function (title,cont,callback) {
		$.post(
			"/api/sendpsg",
			{
				"user"   : s.user,
				"token"  : s.token,
				"title"  : title,
				"content": cont
			},
			function (res) {
				if(res=="noarg!"){
					alert("参数异常");
				}else
				if(res=="nologin!"){
					alert("未登录");
				}else{
					alert("发布成功");
					if(callback)callback(res);
				}
			}
		);
	};
	s.del=function (id,callback) {
		$.post(
			"/api/delpsg",
			{
				"id"     : id,
				"user"   : s.user,
				"token"  : s.token
			},
			function (res) {
				if(res=="noarg!"){
					alert("参数异常");
				}else
				if(res=="nologin!"){
					alert("未登录");
				}else{
					alert("删除成功");
					if(callback)callback();
				}
			}
		);
	};
	s.get=function (id,callback) {
		$.post(
			"/api/getpsg",
			{
				"id":id
			},
			function (res) {
				if(res=="noarg!"){
					alert("参数异常");
				}else
				if(res=="notexist!"){
					alert("文章不存在");
				}else{
					try{
						var data=eval('('+res+')');
						if(callback)callback(data);
					}catch (e) {
						
					}
				}
			}
		);
	};
	s.edit=function (id,title,cont,callback) {
		$.post(
			"/api/editpsg",
			{
				"id"     : id,
				"user"   : s.user,
				"token"  : s.token,
				"title"  : title,
				"content": cont
			},
			function (res) {
				if(res=="noarg!"){
					alert("参数异常");
				}else
				if(res=="nologin!"){
					alert("未登录");
				}else
				if(res=="fail!"){
					alert("发布失败");
				}else{
					alert("发布成功");
					if(callback)callback();
				}
			}
		);
	};
	return s;
}