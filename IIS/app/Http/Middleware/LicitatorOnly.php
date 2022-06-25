<?php

namespace App\Http\Middleware;

use Closure;
use Illuminate\Http\Request;

class LicitatorOnly
{
    /**
     * Handle an incoming request.
     *
     * @param  \Illuminate\Http\Request  $request
     * @param  \Closure  $next
     * @return mixed
     */
    public function handle(Request $request, Closure $next)
    {
        if(auth()->user() == null)
        {
            abort(403);
        }
        if(auth()->user()->licitator == false)
        {
            abort(403);
        }
        return $next($request);
    }
}
