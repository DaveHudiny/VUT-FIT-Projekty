<?php

namespace App\Http\Middleware;

use Closure;
use Illuminate\Http\Request;

class AdminsOnly
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
        if(auth()->user()->username != "Admin")
        {
            abort(403);
        }
        return $next($request);
    }
}
