rift linux init
===============

rift is a very light & simple linux distribution.

Its init is inspired by the NetBSD init and runit.


sf
--

- configure through `/etc/sf.conf`

- service dirs in `/libexec/sf.d/*`
  - and maybe `/usr/pkg/something`

- main interface `/bin/sf`
  - source /etc/sf.conf


/etc/sf.conf:
  if [ -r /etc/defaults/sf.conf ]; then
    . /etc/defaults/sf.conf
  fi
  config vars...


/etc/service/postfix:
  cmd=/usr/sbin/postfix
