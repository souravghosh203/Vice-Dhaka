import React, { useState } from 'react';
import { createRoot } from 'react-dom/client';
import { flushSync } from 'react-dom';

const h = React.createElement;

function GameInterface() {
  const [camera, setCamera] = useState('Chase');
  const [audio, setAudio] = useState(false);
  const [weather, setWeather] = useState(false);
  const [quality, setQuality] = useState('Medium');
  const [toast, setToast] = useState('');
  const [started, setStarted] = useState(false);
  const [paused, setPaused] = useState(false);
  const [mapOpen, setMapOpen] = useState(false);
  const [pauseStats, setPauseStats] = useState('');

  window.GameUI = {
    setCamera: value => setCamera(value),
    setAudio: value => setAudio(value),
    setWeather: value => setWeather(value),
    setQuality: value => setQuality(value),
    setToast: value => setToast(value),
    setStarted: value => setStarted(value),
    setPaused: value => setPaused(value),
    setMapOpen: value => setMapOpen(value),
    setPauseStats: value => setPauseStats(value)
  };

  return h(React.Fragment, null,
    h('div', { className: 'hud', id: 'hud' },
      h('div', { className: 'top' },
        h('section', { className: 'card stats-card' },
          h('div', { id: 'money', className: 'money-value' }),
          h('div', { className: 'health-title' }, h('span', { id: 'modeName' }, 'ON FOOT'), h('span', { id: 'healthText' })),
          h('div', { className: 'stat' }, h('i', { id: 'healthbar' })),
          h('div', { id: 'heat', className: 'heat-value' })
        ),
        h('div', { id: 'job', className: 'card job-card' }),
        h('div', { className: 'map-stack' },
          h('canvas', { className: 'mini', id: 'mini', width: 180, height: 180, 'aria-label': 'Mini map' }),
          h('button', { id: 'mapBtn', className: 'option-button icon-only map-expand', type: 'button', title: 'Open city map', 'aria-label': 'Open city map' }, h('span', { className: 'option-icon' }, '▦'))
        )
      ),
      h('div', { className: 'bottom' },
        h('section', { className: 'card speed-card' },
          h('div', { className: 'speed' }, h('span', { id: 'speed' }), h('small', null, 'KM/H')),
          h('div', { id: 'district', className: 'district-name' })
        ),
        h('div', { className: 'card keyboard-hints' }, 'W/S MOVE | A/D TURN | SHIFT RUN', h('br'), 'J PUNCH | K KICK | E VEHICLE')
      )
    ),
    h('nav', { className: 'actions', 'aria-label': 'Game options' },
      h('button', { id: 'camera', className: 'option-button icon-only', type: 'button', title: 'Change camera angle', 'aria-label': 'Change camera angle' }, h('span', { className: 'option-icon' }, '◎')),
      h('button', { id: 'weather', className: 'option-button icon-only', type: 'button', title: 'Toggle weather', 'aria-label': 'Toggle weather' }, h('span', { className: 'option-icon' }, weather ? '☂' : '☀')),
      h('button', { id: 'sound', className: 'option-button icon-only', type: 'button', title: 'Toggle audio', 'aria-label': 'Toggle audio' }, h('span', { className: 'option-icon' }, audio ? '♫' : '♪'))
    ),    h('button', { id: 'vehicleAction', className: 'vehicle-action hidden', type: 'button' }, 'ENTER VEHICLE'),
    h('div', { className: 'touch' },
      h('div', { className: 'touchgroup' }, h('button', { type: 'button', 'data-key': 'left', 'aria-label': 'Turn left' }, 'L'), h('button', { type: 'button', 'data-key': 'right', 'aria-label': 'Turn right' }, 'R')),
      h('div', { className: 'touchgroup' }, h('button', { type: 'button', 'data-key': 'brake', className: 'pedal', 'aria-label': 'Move back or brake' }, 'DOWN'), h('button', { type: 'button', 'data-key': 'gas', className: 'pedal', 'aria-label': 'Move or accelerate' }, 'UP')),
      h('div', { className: 'touchgroup utility' }, h('button', { type: 'button', 'data-key': 'interact', 'aria-label': 'Enter or exit vehicle' }, 'E'), h('button', { type: 'button', 'data-key': 'punch', 'aria-label': 'Punch' }, 'J'), h('button', { type: 'button', 'data-key': 'kick', 'aria-label': 'Kick' }, 'K'))
    ),    h('div', { id: 'toast', className: 'game-toast' }, toast),
    h('section', { className: `overlay ${started ? 'hidden' : ''}`, id: 'start' },
      h('div', { className: 'panel start-panel' },
        h('div', { className: 'eyebrow' }, 'DHAKA DIVISION · OPEN CITY'),
        h('h1', null, 'VICE ', h('span', null, 'DHAKA')),
        h('h2', null, 'Dhaka Open World'),
        h('p', null, 'Explore Dhaka on foot as Shakib Al Hasan. Walk, sprint, fight, or take a rickshaw, cycle, motorbike, private car, or sports car.'),
        h('p', { className: 'small-copy' }, 'Explore a colorful, original city inspired by Dhaka. Dodge traffic, find landmarks, and earn taka.'),
        h('div', { className: 'row start-actions' },
          h('button', { className: 'primary', id: 'startBtn', type: 'button' }, 'START GAME', h('span', null, '→')),
          h('button', { id: 'settingsBtn', className: 'quality-button', type: 'button' }, 'GRAPHICS', h('span', { id: 'qualityName' }, quality))
        ),
        h('div', { className: 'control-note' }, 'W/S MOVE | A/D TURN | SHIFT RUN | J PUNCH | K KICK | E VEHICLE', h('span', null, '•'), 'MOBILE  ON-SCREEN CONTROLS')
      )
    ),
    h('section', { className: `overlay ${paused ? '' : 'hidden'}`, id: 'pause' },
      h('div', { className: 'panel pause-panel' },
        h('div', { className: 'eyebrow' }, 'TAKE A BREATHER'), h('h2', null, 'RIDE PAUSED'),
        h('p', { id: 'pauseStats' }, pauseStats),
        h('div', { className: 'row' },
          h('button', { className: 'primary', id: 'resume', type: 'button' }, 'RESUME GAME'),
          h('button', { id: 'settings2', className: 'quality-button', type: 'button' }, 'GRAPHICS', h('span', { className: 'qname' }, quality)),
          h('button', { id: 'restart', type: 'button' }, 'RESTART')
        )
      )
    ),
    h('section', { className: `mapcover ${mapOpen ? '' : 'hidden'}`, id: 'mapCover' },
      h('div', { className: 'mapbox' },
        h('canvas', { id: 'map', width: 900, height: 900, 'aria-label': 'Dhaka city map' }),
        h('div', { className: 'maplabel' }, 'VICE DHAKA', h('span', null, 'TAP A LANDMARK TO SET WAYPOINT')),
        h('button', { id: 'closeMap', className: 'map-close', type: 'button' }, 'CLOSE  ×')
      )
    )
  );
}

flushSync(() => createRoot(document.getElementById('ui-root')).render(h(GameInterface)));
