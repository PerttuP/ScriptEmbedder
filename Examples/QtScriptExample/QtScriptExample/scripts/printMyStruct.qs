function main(){
   var x = {foo:"", bar:"", color:"w"};
   x.foo = argv[0];
   x.bar = argv[1];
   x.baz = argv[2];
   var x2 = HostAppAPI.doStuffToMyStruct(x);
   HostAppAPI.saySomething("My struct:", 1);
   HostAppAPI.saySomething(x2.foo, 1);
   HostAppAPI.saySomething(x2.bar, 1);
   HostAppAPI.saySomething(x2.baz, 1);
   return 2;
}

main();