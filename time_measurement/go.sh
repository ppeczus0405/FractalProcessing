#!/bin/bash
mode=("gpu" "cpu")
fractal=("mandelbrot" "julia" "newton")
iters=("100" "500" "1000")
resolution=("1920x1080" "3840x2160" "7680x4320" "15360x8640")

for ((i=0; i<2; i++)); do
  m_mode=${mode[i]}
  for ((j=0; j<4; j++)); do
    m_width="$(awk -F 'x' '{print $1}' <<< "${resolution[j]}")"
    m_height="$(awk -F 'x' '{print $2}' <<< "${resolution[j]}")"
    m_cpu_processes=()
    for ((k=0; k<3; k++)); do
      m_iter=${iters[k]}
      for ((l=0; l<3; l++)); do
        m_fractal=${fractal[l]}
        if [[ "${m_mode}" == "cpu" ]]; then
          ./fractal_benchmark --fractal ${m_fractal} --mode ${m_mode} --width ${m_width} --height ${m_height} --iterations ${m_iter} &
          m_cpu_processes+=("$!")
        else
          ./fractal_benchmark --fractal ${m_fractal} --mode ${m_mode} --width ${m_width} --height ${m_height} --iterations ${m_iter}
        fi
      done
    done
    for ind in ${!m_cpu_processes[@]}; do
      wait ${m_cpu_processes[ind]}
    done
  done

done 
